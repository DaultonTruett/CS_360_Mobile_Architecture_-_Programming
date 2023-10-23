package com.zybooks.weighttracker;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.zybooks.weighttracker.model.Entry;

import java.util.ArrayList;
import java.util.List;

public class EntryAdapter extends RecyclerView.Adapter<EntryAdapter.EntryHolder>{
    private List<Entry> entries = new ArrayList<>();
    private OnItemClickListener listener;

    @NonNull
    @Override
    public EntryHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.weight_entries, parent, false);

        return new EntryHolder(itemView);
    }

    @Override
    public void onBindViewHolder(@NonNull EntryHolder holder, int position) {
        Entry currentEntry = entries.get(position);
        holder.textViewID.setText(String.valueOf(currentEntry.getID()));
        holder.textViewDate.setText(currentEntry.getDate());
        holder.textViewWeight.setText(currentEntry.getWeight());
        holder.itemView.setOnLongClickListener(new View.OnLongClickListener(){
            @Override
            public boolean onLongClick(View view){
                return true;
            }
        });
    }

    @Override
    public int getItemCount() {
        return entries.size();
    }

    public void setEntries(List<Entry> entries){
        this.entries = entries;
        notifyDataSetChanged();
    }

    public Entry getEntryAtPos(int position){
        return entries.get(position);
    }

    class EntryHolder extends RecyclerView.ViewHolder{
        private TextView textViewDate;
        private TextView textViewWeight;
        private TextView textViewID;

        public EntryHolder(@NonNull View itemView) {
            super(itemView);
            textViewID = itemView.findViewById(R.id.newID);
            textViewDate = itemView.findViewById(R.id.newDate);
            textViewWeight = itemView.findViewById(R.id.newWeight);

            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    int position = getAbsoluteAdapterPosition();
                    if(listener != null && position != RecyclerView.NO_POSITION){
                        listener.onItemClick(entries.get(position));
                    }
                }
            });
        }
    }

    public interface OnItemClickListener{
        void onItemClick(Entry entry);
    }

    public void setOnItemClickListener(OnItemClickListener listener){
        this.listener = listener;
    }
}
